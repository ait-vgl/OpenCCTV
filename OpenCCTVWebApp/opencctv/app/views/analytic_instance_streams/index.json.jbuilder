json.array!(@analytic_instance_streams) do |analytic_instance_stream|
  json.extract! analytic_instance_stream, :id, :analytic_instance_id, :analytic_input_stream_id, :stream_id
  json.url analytic_instance_stream_url(analytic_instance_stream, format: :json)
end
