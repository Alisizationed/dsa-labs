import { useEffect, useState } from "react";

import { Card, CardContent } from "../components/ui/card";
import { Button } from "../components/ui/button";

import {
  RadioGroup,
  RadioGroupItem,
} from "../components/ui/radio-group";

import { Label } from "../components/ui/label";

import { getQuestions, saveResult, type Question } from "../data/storage";

export default function QuizSection() {
  const [questions, setQuestions] = useState<Question[]>([]);
  const [answers, setAnswers] = useState<Record<number, string>>({});
  const [score, setScore] = useState<number | null>(null);

  useEffect(() => {
    setQuestions(getQuestions());
  }, []);

  const handleAnswer = (index: number, value: string) => {
    setAnswers((prev) => ({
      ...prev,
      [index]: value,
    }));
  };

  const calculateScore = () => {
    let total = 0;

    questions.forEach((q, i) => {
      if (answers[i] === q.answer) total++;
    });

    saveResult({
      score: total,
      total: questions.length,
      date: new Date().toISOString(),
    });

    setScore(total);
  };

  return (
    <div className="flex justify-center mt-10 px-4">
      <Card className="w-full max-w-4xl bg-white/70 dark:bg-neutral-950/60 backdrop-blur-xl border border-white/20 dark:border-white/10 shadow-xl rounded-2xl">
        <CardContent className="p-8">
          <h1 className="text-3xl font-bold mb-8">
            Media Literacy Quiz
          </h1>

          <div className="space-y-10">
            {questions.map((q, index) => (
              <div
                key={index}
                className="pb-6 border-b border-white/10 dark:border-white/5"
              >
                <h2 className="text-lg font-semibold mb-5">
                  {index + 1}. {q.question}
                </h2>

                <RadioGroup
                  value={answers[index]}
                  onValueChange={(val) =>
                    handleAnswer(index, val)
                  }
                  className="space-y-3"
                >
                  {q.options.map((option) => {
                    const selected =
                      answers[index] === option;

                    return (
                      <div key={option} className="relative">
                        <RadioGroupItem
                          value={option}
                          id={`${index}-${option}`}
                          className="sr-only"
                        />

                        <Label
                          htmlFor={`${index}-${option}`}
                          className={`
                            flex items-center gap-3 rounded-xl px-4 py-3 border cursor-pointer transition-all duration-200
                            backdrop-blur-md
                            ${
                              selected
                                ? "border-blue-500 bg-blue-500/10 shadow-md"
                                : "border-white/10 dark:border-white/5 bg-white/40 dark:bg-neutral-900/40 hover:-translate-y-0.5 hover:shadow-md"
                            }
                          `}
                        >
                          <span
                            className={`
                              h-4 w-4 rounded-full border flex items-center justify-center
                              ${
                                selected
                                  ? "border-blue-500 bg-blue-500"
                                  : "border-neutral-400"
                              }
                            `}
                          >
                            {selected && (
                              <span className="h-2 w-2 bg-white rounded-full" />
                            )}
                          </span>

                          <span className="text-sm">
                            {option}
                          </span>
                        </Label>
                      </div>
                    );
                  })}
                </RadioGroup>
              </div>
            ))}
          </div>

          <Button
            onClick={calculateScore}
            className="mt-10 w-full rounded-xl bg-gradient-to-r from-blue-500 to-indigo-600 text-white font-semibold shadow-lg hover:shadow-xl transition-all"
          >
            Submit Quiz
          </Button>

          {score !== null && (
            <div className="mt-6 rounded-2xl p-6 text-center text-xl font-bold bg-white/60 dark:bg-neutral-900/60 backdrop-blur-xl border border-white/20">
              Your Score: {score} / {questions.length}
            </div>
          )}
        </CardContent>
      </Card>
    </div>
  );
}