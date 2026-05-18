export type Question = {
  question: string;
  type: "multiple" | "boolean";
  options: string[];
  answer: string;
};