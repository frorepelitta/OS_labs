FROM python:3.11
WORKDIR /app
RUN pip install python-telegram-bot==13.15 psycopg2-binary
COPY bot.py .
CMD ["python", "bot.py"]