json.array!(@analytic_instances) do |analytic_instance|
  json.extract! analytic_instance, :id, :analytic_id, :name, :description
  json.url analytic_instance_url(analytic_instance, format: :json)
end
