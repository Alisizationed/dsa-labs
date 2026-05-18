# Newspaper OCR API (Python)

FastAPI service that extracts text from newspaper photos using either **Claude** (cloud) or **Ollama** (local) as the vision backend.

## Setup

```bash
pip install -r requirements.txt
cp .env.example .env   # then edit .env with your settings
uvicorn main:app --reload
```

## Provider switch

Edit `.env` (or set environment variables):

```env
# Use Google Gemini
GOOGLE_API_KEY=...
GOOGLE_MODEL=gemini-flash-latest
```

## Endpoints

### `POST /api/ocr/file` — upload an image file

```bash
curl -X POST http://localhost:8000/api/ocr/file \
     -F "image=@newspaper.jpg"
```

### `POST /api/ocr/url` — provide a public image URL

```bash
curl -X POST http://localhost:8000/api/ocr/url \
     -H "Content-Type: application/json" \
     -d '{"image_url": "https://example.com/newspaper.jpg"}'
```

## Response

```json
{
  "success": true,
  "text": "[HEADLINE]\nBreaking news...\n\n[ARTICLE]\nFull article text...",
  "model": "gemini-flash-latest",
  "input_tokens": 1420,
  "output_tokens": 312
}
```

## Interactive docs

Visit `http://localhost:8000/docs` for the auto-generated Swagger UI.
