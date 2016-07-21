json.array!(@requests) do |request|
  json.extract! request, :id, :status_id, :org_id
  json.url request_url(request, format: :json)
end
