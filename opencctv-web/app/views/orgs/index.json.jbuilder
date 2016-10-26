json.array!(@orgs) do |org|
  json.extract! org, :id, :title, :detail, :password, :org_user_id
  json.url org_url(org, format: :json)
end
