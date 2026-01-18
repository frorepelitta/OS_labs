import os
import psycopg2
import time
from telegram import Update
from telegram.ext import Updater, CommandHandler

TOKEN = os.getenv("BOT_TOKEN")

time.sleep(5)
def get_db():
    return psycopg2.connect(
        host="postgres",
        dbname="telegram_bot",
        user="postgres",
        password="12345"
    )

conn = get_db()
cur = conn.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS tg_messages (message TEXT)")
conn.commit()
cur.close()
conn.close()

def add(update, context):
    text = ' '.join(context.args) if context.args else " "
    conn = get_db()
    conn.cursor().execute("INSERT INTO tg_messages (message) VALUES (%s)", (text,))
    conn.commit()
    conn.close()
    update.message.reply_text(f"Сохранено: {text}")

def list_all(update, context):
    conn = get_db()
    cur = conn.cursor()
    cur.execute("SELECT * FROM tg_messages")
    rows = cur.fetchall()
    conn.close()
    
    result = "\n".join([f"- {message}" for message, in rows])
    update.message.reply_text(f"Записи:\n{result}")

updater = Updater(TOKEN, use_context=True)
updater.dispatcher.add_handler(CommandHandler("add", add))
updater.dispatcher.add_handler(CommandHandler("list", list_all))

updater.start_polling()
updater.idle()

#docker-compose exec postgres psql -U postgres -d telegram_bot -c "..."