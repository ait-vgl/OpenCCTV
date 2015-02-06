json.array!(@analytic_input_streams) do |analytic_input_stream|
  json.extract! analytic_input_stream, :id, :name, :description, :analytic_id
  json.url analytic_input_stream_url(analytic_input_stream, format: :json)
end
