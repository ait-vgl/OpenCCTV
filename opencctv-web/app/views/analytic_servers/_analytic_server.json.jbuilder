json.extract! analytic_server, :id, :name, :ip, :port, :status, :pid, :created_at, :updated_at
json.url analytic_server_url(analytic_server, format: :json)