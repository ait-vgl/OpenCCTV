import MySQLdb


class OpenCCTVdb:
    def __init__(self):
        global db_host, db_username, db_password, db_name_devel

        db_host = 'localhost'
        db_username = 'username'
        db_password = 'password'

        db_name_devel = 'opencctv_development'
        db_name_test = 'opencctv_test'

    def connect(self):
        global conn
        global cursor
        conn = MySQLdb.connect(db_host, db_username, db_password, db_name_devel)
        cursor = conn.cursor()

    def disconnect(self):
        conn.close();

    def get_google_api_keys(self, analytic_instance_id):
        sql = 'SELECT google_api_key FROM mobile_applications' \
              '  INNER JOIN mobile_application_analytics ' \
              '    ON mobile_applications.id = mobile_application_analytics.mobile_application_id' \
              '  INNER JOIN analytic_instances ' \
              '    ON mobile_application_analytics.analytic_id = analytic_instances.analytic_id' \
              '  WHERE analytic_instances.id =  + ' + str(analytic_instance_id)
        cursor.execute(sql)
        try:
            results = cursor.fetchall()
            api_keys = []
            for item in results:
                api_keys.append(item[0])
            return api_keys
        except MySQLdb.Error as e:
            print("SQL Error")
            return []

    def get_google_project_numbers(self, analytic_instance_id):
        sql = 'SELECT google_project_number FROM mobile_applications' \
              '  INNER JOIN mobile_application_analytics' \
              '    ON mobile_applications.id = mobile_application_analytics.mobile_application_id' \
              '  INNER JOIN analytic_instances' \
              '    ON mobile_applications_analytics.analytic_id = analytic_instances.analytic_id' \
              '  WHERE analytic_instances.id = ' + str(analytic_instance_id)
        cursor.execute(sql)
        try:
            results = cursor.fetchall()
            project_numbers = []
            for item in results:
                project_numbers.append(item[0])
            return project_numbers
        except MySQLdb.Error as e:
            print("SQL Error")
            return []

    def get_registration_tokens(self, analytic_instance_id):
        sql = 'SELECT google_tokens.reg_token FROM google_tokens' \
              '  INNER JOIN analytic_instances' \
              '    ON google_tokens.user_id = analytic_instances.user_id' + \
              '  WHERE analytic_instances.id = ' + str(analytic_instance_id)
        cursor.execute(sql)
        try:
            results = cursor.fetchall()
            registration_tokens = []
            for item in results:
                registration_tokens.append(item[0])
            return registration_tokens
        except MySQLdb.Error as e:
            print("SQL Error")
            return []

    def insert_unnotified_messages(self, analytic_instance_id, notification_message, timestamp, google_api_key):
        sql = 'SELECT id FROM mobile_applications' \
              '  WHERE google_api_key = \'' + google_api_key + '\''

        cursor.execute(sql)
        mobile_application_id = cursor.fetchall()[0][0]

        sql = 'SELECT user_id FROM analytic_instances' \
              '  WHERE id = ' + str(analytic_instance_id)
        cursor.execute(sql)
        userid = cursor.fetchall()[0][0]

        sql = 'INSERT INTO unnotified_messages(analytic_instance_id, mobile_application_id, notification_message, timestamp, user_id)' \
              '  VALUES (' \
              '     ' + str(analytic_instance_id) + ',' + \
              '     ' + str(mobile_application_id) + ',' + \
              '     ' + '\'' + str(notification_message) + '\'' + ',' + \
              '     ' + '\'' + timestamp + '\'' + ',' + \
              '     ' + str(userid) + ')'

        try:
            cursor.execute(sql)
            conn.commit()
            print("Written to unnotified_message")
            return 0
        except Exception as e:
            print("SQL Error", e)
            conn.rollback()
            return -1
