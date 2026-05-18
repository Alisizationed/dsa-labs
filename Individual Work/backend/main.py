from fastapi import FastAPI, File, UploadFile, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, HttpUrl

from ocr_service import OcrService
from analysis_service import AnalysisService
from config import settings

app = FastAPI(title="Newspaper OCR & Electoral Integrity API")

# -------------------- CORS --------------------
app.add_middleware(
    CORSMiddleware,
    allow_origins=[
        "http://localhost:5173",
        "http://127.0.0.1:5173",
    ],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
# ---------------------------------------------

ocr = OcrService(settings)
analyser = AnalysisService(settings)


class UrlRequest(BaseModel):
    image_url: HttpUrl


class TextAnalysisRequest(BaseModel):
    text: str


# ---------------- OCR endpoints ----------------

@app.post("/api/ocr/file")
async def ocr_from_file(image: UploadFile = File(...)):
    if not image.content_type.startswith("image/"):
        raise HTTPException(status_code=400, detail="File must be an image")

    image_bytes = await image.read()
    return ocr.extract(image_bytes=image_bytes, mime_type=image.content_type)


@app.post("/api/ocr/url")
async def ocr_from_url(request: UrlRequest):
    return ocr.extract(image_url=str(request.image_url))


# ---------------- Analysis endpoint ----------------

@app.post("/api/analyse")
async def analyse_text(request: TextAnalysisRequest):
    return analyser.analyse(request.text)


# ---------------- Combined pipeline ----------------

@app.post("/api/scan/file")
async def scan_file(image: UploadFile = File(...)):
    if not image.content_type.startswith("image/"):
        raise HTTPException(status_code=400, detail="File must be an image")

    image_bytes = await image.read()

    ocr_result = ocr.extract(
        image_bytes=image_bytes,
        mime_type=image.content_type
    )

    if not ocr_result.get("success"):
        return ocr_result

    analysis_result = analyser.analyse(ocr_result["text"])

    return {
        "success": True,
        "ocr": ocr_result,
        "analysis": analysis_result,
    }


@app.post("/api/scan/url")
async def scan_url(request: UrlRequest):
    ocr_result = ocr.extract(image_url=str(request.image_url))

    if not ocr_result.get("success"):
        return ocr_result

    analysis_result = analyser.analyse(ocr_result["text"])

    return {
        "success": True,
        "ocr": ocr_result,
        "analysis": analysis_result,
    }