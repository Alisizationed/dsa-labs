import { useEffect, useState } from "react";
import { Card, CardContent } from "../components/ui/card";
import {
  getQuestions,
  saveQuestions,
  getResults,
  type Question,
} from "../data/storage";

export default function AdminPanel() {
  const [questions, setQuestions] = useState<Question[]>([]);
  const [results, setResults] = useState<any[]>([]);

  const [form, setForm] = useState<Question>({
    question: "",
    options: ["", "", "", ""],
    answer: "",
  });

  useEffect(() => {
    setQuestions(getQuestions());
    setResults(getResults());
  }, []);

  const handleAdd = () => {
    if (!form.question || !form.answer) return;
    const current = getQuestions();

    const updated = [...current, form];

    saveQuestions(updated);

    setQuestions(updated);

    setForm({
      question: "",
      options: ["", "", "", ""],
      answer: "",
    });
  };

  return (
    <div className="flex justify-center mt-10 px-4">
      <Card className="w-full max-w-5xl">
        <CardContent className="p-8 space-y-10">

          <h1 className="text-3xl font-bold">
            Admin Dashboard
          </h1>

          {/* ================= ADD QUESTION ================= */}
          <div className="border rounded-xl p-6 space-y-4">
            <h2 className="text-xl font-semibold">
              Add Question
            </h2>

            <input
              className="w-full border p-2 rounded"
              placeholder="Question"
              value={form.question}
              onChange={(e) =>
                setForm({ ...form, question: e.target.value })
              }
            />

            {form.options.map((opt, i) => (
              <input
                key={i}
                className="w-full border p-2 rounded mt-2"
                placeholder={`Option ${i + 1}`}
                value={opt}
                onChange={(e) => {
                  const newOptions = [...form.options];
                  newOptions[i] = e.target.value;
                  setForm({ ...form, options: newOptions });
                }}
              />
            ))}

            <input
              className="w-full border p-2 rounded"
              placeholder="Correct answer"
              value={form.answer}
              onChange={(e) =>
                setForm({ ...form, answer: e.target.value })
              }
            />

            <button
              onClick={handleAdd}
              className="bg-black text-white px-4 py-2 rounded"
            >
              Add Question
            </button>
          </div>

          {/* ================= QUESTIONS LIST ================= */}
          <div className="border rounded-xl p-6">
            <h2 className="text-xl font-semibold mb-4">
              Existing Questions
            </h2>

            {questions.length === 0 ? (
              <p className="text-gray-500">No questions yet</p>
            ) : (
              questions.map((q, i) => (
                <div key={i} className="border-b py-2">
                  <p className="font-semibold">{q.question}</p>
                  <p className="text-sm text-gray-500">
                    Answer: {q.answer}
                  </p>
                </div>
              ))
            )}
          </div>

          {/* ================= RESULTS ================= */}
          <div className="border rounded-xl p-6">
            <h2 className="text-xl font-semibold mb-4">
              Quiz Results
            </h2>

            {results.length === 0 ? (
              <p className="text-gray-500">No results yet</p>
            ) : (
              results.map((r, i) => (
                <div key={i} className="border-b py-2">
                  <p>
                    Score: {r.score} / {r.total}
                  </p>
                  <p className="text-sm text-gray-500">
                    {new Date(r.date).toLocaleString()}
                  </p>
                </div>
              ))
            )}
          </div>

        </CardContent>
      </Card>
    </div>
  );
}