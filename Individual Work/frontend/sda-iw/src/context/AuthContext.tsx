import { createContext, useContext, useState, type ReactNode } from "react";
import { users } from "../data/users";

type User = {
  username: string;
  role: "admin" | "user";
};

type AuthContextType = {
  user: User | null;
  login: (username: string, password: string) => boolean;
  logout: () => void;
};

const AuthContext = createContext<AuthContextType | null>(null);

export function AuthProvider({ children }: { children: ReactNode }) {
  const [user, setUser] = useState<User | null>(null);

  const login = (username: string, password: string): boolean => {
    const found = users.find(
      (u) => u.username === username && u.password === password
    );

    if (!found) return false;

    setUser({
      username: found.username,
      role: found.role,
    });

    return true;
  };

  const logout = () => setUser(null);

  return (
    <AuthContext.Provider value={{ user, login, logout }}>
      {children}
    </AuthContext.Provider>
  );
}

export function useAuth() {
  const context = useContext(AuthContext);
  if (!context) throw new Error("useAuth must be used inside AuthProvider");
  return context;
}