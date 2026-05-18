export type UserRole = "admin" | "user";

export type User = {
  username: string;
  role: UserRole;
};