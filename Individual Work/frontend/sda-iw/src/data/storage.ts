export type Question = {
  question: string;
  options: string[];
  answer: string;
};

export type Result = {
  score: number;
  total: number;
  date: string;
};

const QUESTIONS_KEY = "quiz_questions";
const RESULTS_KEY = "quiz_results";

/* ---------------- DEFAULT QUESTIONS ---------------- */

const defaultQuestions: Question[] = [
  {
    question: "What is misinformation?",
    options: [
      "False information shared intentionally to harm",
      "False information shared accidentally",
      "Government verified news",
      "Scientific fact"
    ],
    answer: "False information shared accidentally",
  },
  {
    question: "Which of the following is a sign of fake news?",
    options: [
      "Multiple reliable sources confirm it",
      "Sensational headlines with no sources",
      "Published in scientific journals",
      "Verified by experts"
    ],
    answer: "Sensational headlines with no sources",
  },
  {
    question: "What does media literacy help you do?",
    options: [
      "Believe everything online",
      "Critically evaluate information",
      "Avoid all news",
      "Spread content faster"
    ],
    answer: "Critically evaluate information",
  },
  {
    question: "Disinformation is:",
    options: [
      "Accidental false information",
      "Intentional false information to deceive",
      "Always true information",
      "Only political ads"
    ],
    answer: "Intentional false information to deceive",
  },
];

/* ---------------- QUESTIONS API ---------------- */

export function getQuestions(): Question[] {
  const stored = localStorage.getItem(QUESTIONS_KEY);

  if (!stored) {
    localStorage.setItem(
      QUESTIONS_KEY,
      JSON.stringify(defaultQuestions)
    );
    return defaultQuestions;
  }

  return JSON.parse(stored);
}

export function saveQuestions(q: Question[]) {
  localStorage.setItem(QUESTIONS_KEY, JSON.stringify(q));
}

/* ---------------- RESULTS API ---------------- */

export function saveResult(result: Result) {
  const existing = JSON.parse(
    localStorage.getItem(RESULTS_KEY) || "[]"
  );

  existing.push(result);

  localStorage.setItem(
    RESULTS_KEY,
    JSON.stringify(existing)
  );
}

export function getResults(): Result[] {
  return JSON.parse(
    localStorage.getItem(RESULTS_KEY) || "[]"
  );
}