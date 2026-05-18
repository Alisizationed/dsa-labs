import { useState } from "react";
import FileUpload from "./FileUpload";
import { Card, CardContent } from "../components/ui/card";

type Flag = {
  category: string;
  confidence: number;
  excerpts: string[];
  explanation: string;
};

type InnerAnalysis = {
  overall_risk: "none" | "low" | "medium" | "high" | "critical";
  summary: string;
  flags: Flag[];
  recommendation: "none" | "monitor" | "review" | "escalate";
};

type ApiResponse = {
  success: boolean;
  ocr?: {
    success: boolean;
    text: string;
    model?: string;
    input_tokens?: number;
    output_tokens?: number;
  };
  analysis?: {
    success: boolean;
    skipped_llm: boolean;
    reason?: string;
    heuristic_scores: Record<string, number>;
    analysis: InnerAnalysis;
  };
};

export default function UploadSection() {
  const [result, setResult] = useState<ApiResponse | null>(null);
  const [debug, setDebug] = useState<any>(null);
  const [loading, setLoading] = useState(false);

  const handleUpload = async (files: File[]) => {
    const file = files[0];
    if (!file) return;

    const formData = new FormData();
    formData.append("image", file);

    try {
      setLoading(true);
      setResult(null);
      setDebug(null);

      const res = await fetch("http://127.0.0.1:8000/api/scan/file", {
        method: "POST",
        body: formData,
      });

      const data = await res.json();

      console.log("API RESPONSE:", data);
      setDebug(data);

      if (!res.ok) {
        throw new Error(data?.detail || "Upload failed");
      }

      setResult(data);
    } catch (err) {
      console.error("UPLOAD ERROR:", err);
      alert("Error analyzing file");
    } finally {
      setLoading(false);
    }
  };

  // ✅ SAFE EXTRACTION (prevents crashes)
  const ocrText = result?.ocr?.text ?? "";
  const analysis = result?.analysis?.analysis;
  const flags = analysis?.flags ?? [];

  const skipped = result?.analysis?.skipped_llm;
  const reason = result?.analysis?.reason;

  return (
    <div className="flex justify-center mt-10 px-4">
      <Card className="w-full max-w-3xl">
        <CardContent className="p-8 space-y-6">

          <h1 className="text-3xl font-bold">Upload Newspaper</h1>

          <p className="text-muted-foreground">
            Upload newspapers or articles to analyze misinformation risk.
          </p>

          <FileUpload onChange={handleUpload} />

          {/* LOADING */}
          {loading && (
            <div className="p-4 border rounded-lg text-sm">
              Analyzing file...
            </div>
          )}

          {/* RESULT */}
          {result && (
            <div className="space-y-6 p-4 border rounded-lg">

              {/* OCR */}
              {/* <div>
                <h2 className="text-xl font-semibold mb-2">
                  OCR Result
                </h2>

                <div className="text-sm whitespace-pre-wrap border rounded p-3 max-h-80 overflow-auto">
                  {ocrText || "No text extracted."}
                </div>
              </div> */}

              {/* ANALYSIS */}
              <div>
                <h2 className="text-xl font-semibold mb-2">
                  Analysis Result
                </h2>

                {/* skipped / safe content info */}
                {skipped && (
                  <div className="p-3 mb-3 border rounded bg-blue-50 text-sm">
                    <strong>Skipped full analysis:</strong> {reason}
                  </div>
                )}

                {!analysis ? (
                  <p className="text-sm text-gray-500">
                    No analysis available.
                  </p>
                ) : (
                  <>
                    <p>
                      <strong>Risk:</strong>{" "}
                      <span className="uppercase">
                        {analysis.overall_risk}
                      </span>
                    </p>

                    <p className="mt-2">
                      <strong>Summary:</strong> {analysis.summary}
                    </p>

                    <p className="mt-2">
                      <strong>Recommendation:</strong>{" "}
                      {analysis.recommendation}
                    </p>

                    {/* FLAGS */}
                    {flags.length > 0 && (
                      <div className="mt-4">
                        <h3 className="font-semibold">Flags</h3>

                        <div className="space-y-3 mt-2">
                          {flags.map((flag, i) => (
                            <div key={i} className="border rounded p-3 text-sm">

                              <p>
                                <strong>Category:</strong> {flag.category}
                              </p>

                              <p>
                                <strong>Confidence:</strong> {flag.confidence}
                              </p>

                              <p>
                                <strong>Explanation:</strong> {flag.explanation}
                              </p>

                              {(flag.excerpts ?? []).length > 0 && (
                                <ul className="list-disc ml-5 mt-2">
                                  {(flag.excerpts ?? []).map((e, idx) => (
                                    <li key={idx}>{e}</li>
                                  ))}
                                </ul>
                              )}
                            </div>
                          ))}
                        </div>
                      </div>
                    )}
                  </>
                )}
              </div>
            </div>
          )}

          {/* DEBUG */}
          {/* {debug && (
            <div className="mt-4 p-3 border rounded bg-gray-50 text-xs">
              <h3 className="font-semibold mb-2">Debug Response</h3>
              <pre className="whitespace-pre-wrap overflow-auto max-h-60">
                {JSON.stringify(debug, null, 2)}
              </pre>
            </div>
          )} */}

        </CardContent>
      </Card>
    </div>
  );
}