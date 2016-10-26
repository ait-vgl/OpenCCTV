json.array!(@notifications) do |notification|
  json.extract! notification, :id, :title, :email, :to_user
  json.url notification_url(notification, format: :json)
end
