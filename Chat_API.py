import requests
import json
import os
API_KEY = "uAhrq0GwelKK5F0tyonojc9w"
SECRET_KEY = "vpHMPK25EAbp2Pac5SA8O9TkD2rCVi1U"
MESSAGES = []
def get_access_token():
    url = f"https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id={API_KEY}&client_secret={SECRET_KEY}"
    headers = {'Content-Type': 'application/json'}
    response = requests.post(url, headers=headers)
    return response.json().get("access_token")
def chat(user_input):
    global MESSAGES
    MESSAGES.append({"role": "user", "content": user_input})
    url = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/yi_34b_chat?access_token=" + get_access_token()
    payload = json.dumps({"messages": MESSAGES})
    headers = {'Content-Type': 'application/json'}
    response = requests.post(url, headers=headers, data=payload)
    response_data = response.json()
    MESSAGES.append({"role": "assistant", "content": response_data["result"]})
    return response_data["result"]
def main_loop():
    print("Welcome to the chat! Type 'exit' to quit, if you want to clear the chat history, type 'clear' or 'clear()' .")
    while True:
        user_input = input("\nYou: ")
        if user_input.lower() == 'clear' or user_input.lower() == 'clear()':
            MESSAGES = []
            if user_input.lower()== "clear()":
                os.system('cls')
                os.system('cls')
            print("\n                        Chat history cleared! \n")
            print('*'*16,'-'*84,'*'*16)
            print("Welcome to the chat! Type 'exit' to quit, if you want to clear the chat history, type 'clear' or 'clear()' .")
        elif user_input.lower() == 'exit':
            print("Goodbye!")
            break
        else:
            assistant_reply = chat(user_input)
            print(f"Assistant: {assistant_reply}")
main_loop()