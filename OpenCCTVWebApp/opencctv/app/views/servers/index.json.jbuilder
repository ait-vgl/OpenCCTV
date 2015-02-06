json.array!(@servers) do |server|
  json.extract! server, :id, :name, :host, :port, :status, :username, :password
  json.url server_url(server, format: :json)
end
