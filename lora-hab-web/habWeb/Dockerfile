FROM python:3.9-alpine AS builder

EXPOSE 8000
WORKDIR /app 
COPY lora-hab-web/habWeb/requirements.txt /app
RUN pip3 install -r requirements.txt --no-cache-dir
COPY lora-hab-web/habWeb/ /app
CMD ["/app/config/entrypoint.sh"]