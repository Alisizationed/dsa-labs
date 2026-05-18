import { useState } from "react";
import { useAuth } from "../context/AuthContext";

export default function Login({ onSuccess }: { onSuccess: () => void }) {
  const { login } = useAuth();

  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");

  const handleLogin = () => {
    const ok = login(username, password);

    if (!ok) {
      setError("Invalid username or password");
      return;
    }

    setError("");
    onSuccess();
  };

  return (
    <div className="min-h-screen flex items-center justify-center bg-gradient-to-br from-neutral-50 to-neutral-200 dark:from-neutral-950 dark:to-neutral-900 px-4">
      <div className="w-full max-w-md rounded-2xl border border-white/20 dark:border-white/10 bg-white/60 dark:bg-neutral-900/60 backdrop-blur-xl shadow-2xl p-8">

        {/* HEADER */}
        <h1 className="text-3xl font-bold text-center mb-2">
          Welcome back
        </h1>
        <p className="text-center text-sm text-neutral-500 mb-6">
          Sign in to continue
        </p>

        {/* INPUTS */}
        <div className="space-y-4">
          <input
            className="
              w-full px-4 py-3 rounded-xl
              bg-white/70 dark:bg-neutral-800/60
              border border-white/20 dark:border-white/10
              outline-none focus:ring-2 focus:ring-blue-500
              transition
            "
            placeholder="Username"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />

          <input
            className="
              w-full px-4 py-3 rounded-xl
              bg-white/70 dark:bg-neutral-800/60
              border border-white/20 dark:border-white/10
              outline-none focus:ring-2 focus:ring-blue-500
              transition
            "
            type="password"
            placeholder="Password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
        </div>

        {/* ERROR */}
        {error && (
          <p className="text-red-500 text-sm mt-3 text-center">
            {error}
          </p>
        )}

        {/* BUTTON */}
        <button
          onClick={handleLogin}
          className="
            w-full mt-6 py-3 rounded-xl
            bg-gradient-to-r from-blue-500 to-indigo-600
            text-white font-semibold
            shadow-lg hover:shadow-xl
            hover:-translate-y-0.5
            transition-all
          "
        >
          Sign in
        </button>

        {/* FOOTER HINT */}
        {/* <p className="text-xs text-center text-neutral-500 mt-4">
          admin/admin123 or user/user123
        </p> */}
      </div>
    </div>
  );
}