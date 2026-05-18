import * as React from "react";
import * as SelectPrimitive from "@radix-ui/react-select";

import { cn } from "../../lib/utils";
import {
  CaretDownIcon,
  CheckIcon,
} from "@phosphor-icons/react";

/* ================= ROOT ================= */
function Select(
  props: React.ComponentProps<typeof SelectPrimitive.Root>
) {
  return <SelectPrimitive.Root {...props} />;
}

/* ================= TRIGGER ================= */
function SelectTrigger({
  className,
  children,
  ...props
}: React.ComponentProps<typeof SelectPrimitive.Trigger>) {
  return (
    <SelectPrimitive.Trigger
      className={cn(
        // ===== ACETERNITY CORE =====
        "group relative flex w-full items-center justify-between gap-2",
        "rounded-xl px-4 py-2.5 text-sm",

        // glass effect
        "bg-white/70 dark:bg-neutral-950/60 backdrop-blur-xl",
        "border border-white/20 dark:border-white/10",

        // shadow + glow
        "shadow-[0_8px_30px_rgb(0,0,0,0.06)]",
        "transition-all duration-300",

        // hover + focus
        "hover:shadow-[0_12px_40px_rgba(0,0,0,0.12)]",
        "focus:outline-none focus:ring-2 focus:ring-blue-500/30",

        // text
        "text-neutral-700 dark:text-neutral-200",

        className
      )}
      {...props}
    >
      {children}

      <SelectPrimitive.Icon>
        <CaretDownIcon
          className={cn(
            "size-4 opacity-60 transition-transform duration-200",
            "group-data-[state=open]:rotate-180"
          )}
        />
      </SelectPrimitive.Icon>
    </SelectPrimitive.Trigger>
  );
}

/* ================= CONTENT ================= */
function SelectContent({
  className,
  children,
  ...props
}: React.ComponentProps<typeof SelectPrimitive.Content>) {
  return (
    <SelectPrimitive.Portal>
      <SelectPrimitive.Content
        className={cn(
          // ===== ACETERNITY DROPDOWN =====
          "relative z-50 min-w-[10rem] overflow-hidden",

          // glass panel
          "bg-white/70 dark:bg-neutral-950/70 backdrop-blur-2xl",
          "border border-white/20 dark:border-white/10",
          "rounded-2xl",

          // shadow glow
          "shadow-[0_20px_60px_rgba(0,0,0,0.2)]",

          // animation
          "data-[state=open]:animate-in data-[state=closed]:animate-out",
          "data-[state=open]:fade-in-0 data-[state=closed]:fade-out-0",
          "data-[state=open]:zoom-in-95 data-[state=closed]:zoom-out-95",
          "duration-200",

          className
        )}
        {...props}
      >
        <SelectPrimitive.Viewport className="p-1">
          {children}
        </SelectPrimitive.Viewport>
      </SelectPrimitive.Content>
    </SelectPrimitive.Portal>
  );
}

/* ================= ITEM ================= */
function SelectItem({
  className,
  children,
  ...props
}: React.ComponentProps<typeof SelectPrimitive.Item>) {
  return (
    <SelectPrimitive.Item
      className={cn(
        "relative flex cursor-pointer select-none items-center rounded-xl px-3 py-2 text-sm",

        // hover glow
        "hover:bg-white/60 dark:hover:bg-neutral-800/60",
        "transition-colors duration-200",

        // disabled
        "data-[disabled]:opacity-50 data-[disabled]:pointer-events-none",

        // focus ring feel
        "outline-none focus:bg-white/70 dark:focus:bg-neutral-800/70",

        className
      )}
      {...props}
    >
      <SelectPrimitive.ItemText>
        {children}
      </SelectPrimitive.ItemText>

      <span className="absolute right-3 flex items-center">
        <SelectPrimitive.ItemIndicator>
          <CheckIcon className="size-4 text-blue-500" />
        </SelectPrimitive.ItemIndicator>
      </span>
    </SelectPrimitive.Item>
  );
}

/* ================= VALUE ================= */
function SelectValue(
  props: React.ComponentProps<typeof SelectPrimitive.Value>
) {
  return <SelectPrimitive.Value {...props} />;
}

/* ================= EXPORT ================= */
export {
  Select,
  SelectTrigger,
  SelectContent,
  SelectItem,
  SelectValue,
};