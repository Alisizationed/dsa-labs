from google.genai import Client, types

OCR_PROMPT = """This is a photo of a newspaper. Transcribe ALL visible text accurately.
Preserve the structure:
- Mark headlines with [HEADLINE]
- Mark article bodies with [ARTICLE]
- Mark captions with [CAPTION]
- Separate distinct articles with a blank line
Output only the transcribed text, nothing else."""


class OcrService:
    def __init__(self, settings):
        self.client = Client(api_key=settings.google_api_key)
        self.model = settings.google_model

    def extract(self, image_bytes: bytes = None, mime_type: str = "image/jpeg", image_url: str = None) -> dict:
        if image_url:
            media_part = types.Part.from_uri(uri=image_url, mime_type=mime_type)
        else:
            media_part = types.Part.from_bytes(data=image_bytes, mime_type=mime_type)

        response = self.client.models.generate_content(
            model=self.model,
            contents=[OCR_PROMPT, media_part],
            config=types.GenerateContentConfig(
                max_output_tokens=4096,
                temperature=0.0,
            ),
        )

        return {
            "success": True,
            "text": response.text,
            "model": response.model_version,
            "input_tokens": response.usage_metadata.prompt_token_count,
            "output_tokens": response.usage_metadata.candidates_token_count,
        }