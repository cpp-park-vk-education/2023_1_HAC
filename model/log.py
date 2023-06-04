from datetime import datetime

def log(message):
    with open('model/output.txt', 'a') as file:
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        file.write(f"{current_time}: {message}\n")