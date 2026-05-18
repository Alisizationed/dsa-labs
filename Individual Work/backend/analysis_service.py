import json
from google.genai import Client, types
from filter_bridge import needs_llm

SYSTEM_PROMPT = """You are an expert analyst specialising in electoral integrity, 
media literacy, and information warfare. Your role is to objectively assess 
newspaper text for content that may undermine democratic processes.

You must be precise, evidence-based, and non-partisan. Flag only what the text 
actually contains — do not infer intent beyond what is clearly supported."""

ANALYSIS_PROMPT = """Analyse the following newspaper text for electoral integrity concerns.

For each category below, determine if the text contains it, and if so provide:
- A confidence score (0.0–1.0)
- Specific quoted excerpts that triggered the flag
- A brief explanation of why it qualifies

Categories to assess:
1. PROPAGANDA — Emotionally manipulative framing, demonisation of candidates/groups, 
   us-vs-them rhetoric designed to bypass rational thinking.
2. DISINFORMATION — Factually false or fabricated claims presented as truth 
   (verifiably wrong facts, invented statistics, fake quotes).
3. MALINFORMATION — True information used out of context or selectively to mislead 
   (real facts weaponised to cause harm or distort the overall picture).
4. VOTER_SUPPRESSION — Discouraging specific groups from voting (wrong dates, 
   ID misinformation, threats, claims that voting is pointless).
5. ELECTORAL_FRAUD_CLAIMS — Unsubstantiated claims of rigged elections, 
   ballot tampering, or compromised electoral systems without evidence.
6. AGGRESSIVE_PROMOTION — Disproportionate or coercive promotion of a candidate/party 
   that crosses into manipulation rather than legitimate campaigning.
7. HATE_SPEECH — Content targeting voters or candidates based on ethnicity, 
   religion, gender, or other protected characteristics.

Respond ONLY with valid JSON in this exact structure:
{
  "overall_risk": "none|low|medium|high|critical",
  "summary": "2-3 sentence plain-language summary of findings",
  "flags": [
    {
      "category": "CATEGORY_NAME",
      "confidence": 0.0,
      "excerpts": ["quoted text from the article"],
      "explanation": "why this qualifies"
    }
  ],
  "recommendation": "none|monitor|review|escalate"
}

If no concerns are found, return an empty flags array and overall_risk of "none".
Do not include any text outside the JSON object."""


class AnalysisService:
    def __init__(self, settings):
        self.settings = settings
        self.client = Client(api_key=settings.google_api_key)   # Gemini client
        self.model = settings.google_model                      # e.g. "gemini-1.5-flash"

    def analyse(self, text: str) -> dict:
        if not text or not text.strip():
            return {"success": False, "error": "No text provided for analysis"}

        # ── Step 1: C heuristic pre-filter ────────────────────────────────
        escalate, heuristic_scores = needs_llm(text)

        if not escalate:
            # Text is clean — skip the LLM entirely, return immediately
            return {
                "success": True,
                "skipped_llm": True,
                "reason": "Heuristic pre-filter found no signals above threshold",
                "heuristic_scores": heuristic_scores,
                "analysis": {
                    "overall_risk": "none",
                    "summary": "No manipulation signals detected by heuristic analysis.",
                    "flags": [],
                    "recommendation": "none",
                },
            }

        # ── Step 2: Gemini deep analysis (only if pre-filter fires) ──────
        # Combine system prompt and user prompt into a single user message,
        # or use the 'system_instruction' parameter if your SDK version supports it.
        # Here we use the simpler approach: embed system prompt into the user prompt.
        full_prompt = f"{SYSTEM_PROMPT}\n\n{ANALYSIS_PROMPT}\n\nText to analyse:\n\n{text}"

        response = self.client.models.generate_content(
            model=self.model,
            contents=full_prompt,
            config=types.GenerateContentConfig(
                max_output_tokens=2048,
                temperature=0.0,
            ),
        )

        # Extract JSON from response text (Gemini may wrap in ```json ... ```)
        raw_text = response.text.strip()
        if raw_text.startswith("```json"):
            raw_text = raw_text[7:]
        if raw_text.endswith("```"):
            raw_text = raw_text[:-3]
        raw_text = raw_text.strip()

        analysis = json.loads(raw_text)

        return {
            "success": True,
            "skipped_llm": False,
            "heuristic_scores": heuristic_scores,
            "analysis": analysis,
            "model": response.model_version,
            "input_tokens": response.usage_metadata.prompt_token_count,
            "output_tokens": response.usage_metadata.candidates_token_count,
        }