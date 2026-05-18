import {
  Navbar,
  NavBody,
  MobileNav,
  NavbarLogo,
  NavbarButton,
  MobileNavHeader,
  MobileNavToggle,
  MobileNavMenu,
} from "../components/ui/resizable-navbar";

import { useState } from "react";
import UploadSection from "./UploadSection";
import QuizSection from "./QuizSection";
import AdminPanel from "./AdminPanel";
import { useAuth } from "../context/AuthContext";
import Login from "../pages/Login";

type Page = "upload" | "play" | "admin";

export function NavbarDemo() {
  const { user, logout } = useAuth();

  const [page, setPage] = useState<Page>("upload");
  const [isMobileMenuOpen, setIsMobileMenuOpen] = useState(false);
  const [loggedIn, setLoggedIn] = useState(false);

  // 🔐 LOGIN GATE
  if (!loggedIn || !user) {
    return <Login onSuccess={() => setLoggedIn(true)} />;
  }

  const navItems: { name: string; page: Page }[] = [
    { name: "Upload Newspaper", page: "upload" },
    { name: "Play Quiz", page: "play" },
    ...(user.role === "admin"
      ? [{ name: "Admin Panel", page: "admin" }]
      : []),
  ];

  return (
    <div className="relative w-full">
      <Navbar>
        <NavBody>
          <NavbarLogo />

          {/* NAV */}
          <div className="hidden lg:flex gap-2 flex-1 justify-center">
            {navItems.map((item) => (
              <button
                key={item.page}
                onClick={() => setPage(item.page)}
                className="px-4 py-2 text-sm rounded-full hover:bg-neutral-100 dark:hover:bg-neutral-800"
              >
                {item.name}
              </button>
            ))}
          </div>

          {/* USER INFO */}
          <div className="flex items-center gap-3">
            <span className="text-xs text-neutral-500">
              {user.username} ({user.role})
            </span>

            <NavbarButton variant="dark" onClick={logout}>
              Logout
            </NavbarButton>
          </div>
        </NavBody>

        {/* MOBILE */}
        <MobileNav>
          <MobileNavHeader>
            <NavbarLogo />
            <MobileNavToggle
              isOpen={isMobileMenuOpen}
              onClick={() =>
                setIsMobileMenuOpen(!isMobileMenuOpen)
              }
            />
          </MobileNavHeader>

          <MobileNavMenu
            isOpen={isMobileMenuOpen}
            onClose={() => setIsMobileMenuOpen(false)}
          >
            {navItems.map((item) => (
              <button
                key={item.page}
                onClick={() => {
                  setPage(item.page);
                  setIsMobileMenuOpen(false);
                }}
                className="w-full text-left px-3 py-2 rounded-md hover:bg-gray-100 dark:hover:bg-neutral-800"
              >
                {item.name}
              </button>
            ))}

            <button
              onClick={logout}
              className="mt-4 text-red-500 text-left"
            >
              Logout
            </button>
          </MobileNavMenu>
        </MobileNav>
      </Navbar>

      {/* PAGES */}
      <MainPage page={page} />
    </div>
  );
}

function MainPage({ page }: { page: Page }) {
  return (
    <div className="pt-24">
      {page === "upload" && <UploadSection />}
      {page === "play" && <QuizSection />}
      {page === "admin" && <AdminPanel />}
    </div>
  );
}