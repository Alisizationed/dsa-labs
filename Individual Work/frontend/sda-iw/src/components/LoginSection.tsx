import {
  createContext,
  useContext,
  useState,
  type ReactNode,
} from "react";

import { type User } from "../types/user";

type AuthContextType = {
  user: User | null;
  login: (username: string, role: "admin" | "user") => void;
  logout: () => void;
};

const AuthContext = createContext<AuthContextType | null>(
  null
);

export function AuthProvider({
  children,
}: {
  children: ReactNode;
}) {
  const [user, setUser] = useState<User | null>(null);

  const login = (
    username: string,
    role: "admin" | "user"
  ) => {
    setUser({
      username,
      role,
    });
  };

  const logout = () => {
    setUser(null);
  };

  return (
    <AuthContext.Provider
      value={{
        user,
        login,
        logout,
      }}
    >
      {children}
    </AuthContext.Provider>
  );
}

export function useAuth() {
  const context = useContext(AuthContext);

  if (!context) {
    throw new Error(
      "useAuth must be used inside AuthProvider"
    );
  }

  return context;
}