import json
import requests


class GCMSender:

    def __init__(self):
        pass

    def send(self, api_key, reg_token, analytic_instance_id, notification_message, timestamp):
        url = 'https://gcm-http.googleapis.com/gcm/send'

        headers = {
            'Content-Type': 'application/json',
            'Authorization': 'key=' + api_key
        }
        payloads = {
            'to': reg_token,
            'data': {
                'analytic_instance_id': analytic_instance_id,
                'notification_message': notification_message,
                'timestamp': timestamp
            }
        }


        print("\tHeader: "+ str(headers))
        print("\tSending to: " + (reg_token))

        try:
            r = requests.post(url=url, headers=headers, data=json.dumps(payloads))
            print(r.text)
            print("Done")
            return 0
        except Exception as e:
            # Cannot complete POST requests
            print(e)
            return -1
