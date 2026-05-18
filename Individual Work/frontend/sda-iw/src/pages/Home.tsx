import { useState, useEffect } from "react";

import {NavbarDemo} from "../components/NavbarDemo";

import { useAuth } from "../context/AuthContext";

export default function Home() {
  const { user } = useAuth();

  const [page, setPage] = useState<
    "upload" | "play" | "admin"
  >("upload");

  useEffect(() => {
    if (user) setPage("upload");
  }, [user]);

  return (
    <div className="min-h-screen">
      <NavbarDemo page={page} setPage={setPage} />
    </div>
  );
}