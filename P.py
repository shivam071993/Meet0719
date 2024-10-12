from telegram import Update
from telegram.ext import Updater, CommandHandler, CallbackContext
import psutil

# Function to fetch system information
def get_system_info():
    cpu_usage = psutil.cpu_percent(interval=1)
    memory = psutil.virtual_memory()
    disk = psutil.disk_usage('/')

    info = (
        f"CPU Usage: {cpu_usage}%\n"
        f"Memory Usage: {memory.used / (1024 ** 2):.2f} MB used out of {memory.total / (1024 ** 2):.2f} MB ({memory.percent}%)\n"
        f"Disk Usage: {disk.used / (1024 ** 2):.2f} MB used out of {disk.total / (1024 ** 2):.2f} MB ({disk.percent}%)"
    )
    return info

# Command handler for /system
def system_command(update: Update, context: CallbackContext):
    info = get_system_info()
    update.message.reply_text(info)

# Main function to run the bot
def main():
    # Replace 'YOUR_TOKEN' with your actual bot token
    updater = Updater("7740049047:AAGPVttQ1hLSmhU62QKJ2QicdOPPnXFxgJ8")

    # Get the dispatcher to register handlers
    dp = updater.dispatcher

    # Add the /system command handler
    dp.add_handler(CommandHandler("system", system_command))

    # Start the Bot
    updater.start_polling()

    # Run the bot until you press Ctrl-C
    updater.idle()

if __name__ == '__main__':
    main()
