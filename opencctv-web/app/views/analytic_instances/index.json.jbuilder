json.array!(@analytic_instances) do |analytic_instance|
  json.extract! analytic_instance, :id, :name, :description, :analytic_id
  json.url analytic_instance_url(analytic_instance, format: :json)
end
