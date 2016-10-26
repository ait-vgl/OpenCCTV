import xml.etree.ElementTree as ET


class OpenCctvResult:
    def __init__(self):
        global analytic_instance_id, notification_message, timestamp
        analytic_instance_id = 0
        notification_message = ''
        timestamp = ''

    def set_result_message(self, result_message):
        global analytic_instance_id, notification_message
        analytic_instance_id = -1
        notification_message = ''

        args = result_message.split('|')
        for arg in args:
            key_value_pair = arg.split(': ')
            key = key_value_pair[0]
            value = key_value_pair[1]

            if key == 'analyticiid':
                analytic_instance_id = value
            elif key == 'timestamp':
                global timestamp
                timestamp = value
            elif key == 'customtext':
                tree = ET.fromstring(value)
                for item in tree:
                    if item.tag == 'notification-message':
                        notification_message = item.text

    def get_analytic_instance_id(self):
        return int(analytic_instance_id)

    def get_notification_message(self):
        return notification_message

    def get_timestamp(self):
        return timestamp
