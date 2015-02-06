json.array!(@analytics) do |analytic|
  json.extract! analytic, :id, :name, :description, :location
  json.url analytic_url(analytic, format: :json)
end
