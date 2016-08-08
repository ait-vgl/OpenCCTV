#!/usr/bin/python2

import zmq

from utils.db.OpenCCTVdb import OpenCCTVdb
from utils.opencctv.OpenCctvResult import OpenCctvResult
from utils.gcm.GCMSender import GCMSender


def main():
    zmq_context = zmq.Context()
    zmq_socket = zmq_context.socket(zmq.PULL)
    zmq_socket.bind('tcp://127.0.0.1:1234')
    print('\n[INFO] Notification server is running on port 1234')

    opencctv_db = OpenCCTVdb()
    opencctv_result = OpenCctvResult()
    gcm_sender = GCMSender()

    # Receiving result message from message queue
    while True:
        result_message = zmq_socket.recv()
        opencctv_db.connect()
        opencctv_result.set_result_message(result_message)

        analytic_instance_id = opencctv_result.get_analytic_instance_id()
        timestamp = opencctv_result.get_timestamp()
        notification_message = opencctv_result.get_notification_message()

        if analytic_instance_id != '' and notification_message != '':
            print("Sending message: " + notification_message)
            google_api_keys = opencctv_db.get_google_api_keys(analytic_instance_id)

            for google_api_key in google_api_keys:
                google_reg_tokens = opencctv_db.get_registration_tokens(analytic_instance_id)
                if len(google_reg_tokens) == 0:
                    opencctv_db.insert_unnotified_messages(
                        analytic_instance_id,
                        notification_message,
                        timestamp,
                        google_api_key
                    )
                else:
                    for google_reg_token in google_reg_tokens:
                        result = gcm_sender.send(
                            google_api_key,
                            google_reg_token,
                            analytic_instance_id,
                            notification_message,
                            timestamp,
                        )

                        if result == -1:
                            # Push result message back to the message queue.
                            zmq_sender_context = zmq.Context()
                            zmq_sender_socket = zmq_sender_context.socket(zmq.PUSH)
                            zmq_sender_socket.connect('tcp://127.0.0.1:1234')
                            zmq_sender_socket.send(result_message)
        opencctv_db.disconnect()


if __name__ == '__main__':
    main()
