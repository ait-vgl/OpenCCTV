json.array!(@vmses) do |vms|
  json.extract! vms, :id, :server_name, :server_port, :vms_type, :description, :username, :password
  json.url vms_url(vms, format: :json)
end
