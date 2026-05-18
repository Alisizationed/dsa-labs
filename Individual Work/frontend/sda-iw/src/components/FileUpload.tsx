"use client";

import { useRef, useState } from "react";
import { useDropzone } from "react-dropzone";
import { motion } from "motion/react";
import { IconUpload } from "@tabler/icons-react";
import { cn } from "../lib/utils";

type Props = {
  onChange?: (files: File[]) => void;
};

export default function FileUpload({ onChange }: Props) {
  const [files, setFiles] = useState<File[]>([]);
  const inputRef = useRef<HTMLInputElement>(null);

  const handleFiles = (newFiles: File[]) => {
    setFiles((prev) => [...prev, ...newFiles]);
    onChange?.(newFiles);
  };

  const { getRootProps, isDragActive } = useDropzone({
    multiple: false,
    noClick: true,
    onDrop: handleFiles,
  });

  return (
    <div {...getRootProps()} className="w-full">
      <motion.div
        onClick={() => inputRef.current?.click()}
        className="relative cursor-pointer rounded-2xl border bg-background p-10 shadow-sm"
        whileHover={{ scale: 1.01 }}
      >
        <input
          ref={inputRef}
          type="file"
          className="hidden"
          onChange={(e) =>
            handleFiles(Array.from(e.target.files || []))
          }
        />

        {/* Drop area */}
        <div className="flex flex-col items-center justify-center text-center">
          <IconUpload className="h-6 w-6 text-muted-foreground" />

          <p className="mt-3 font-semibold">
            Upload file
          </p>

          <p className="text-sm text-muted-foreground">
            Drag & drop or click to upload
          </p>
        </div>

        {/* File list */}
        {files.length > 0 && (
          <div className="mt-6 space-y-3">
            {files.map((file, idx) => (
              <motion.div
                key={idx}
                className="flex items-center justify-between rounded-lg border bg-muted p-3 text-sm"
                initial={{ opacity: 0, y: 10 }}
                animate={{ opacity: 1, y: 0 }}
              >
                <span className="truncate">
                  {file.name}
                </span>

                <span className="text-xs text-muted-foreground">
                  {(file.size / 1024 / 1024).toFixed(
                    2
                  )}{" "}
                  MB
                </span>
              </motion.div>
            ))}
          </div>
        )}
      </motion.div>
    </div>
  );
}