json.array!(@results) do |result|
  json.extract! result, :id, :analytic_instance_id, :timestamp, :result_text, :time, :count
  json.url result_url(result, format: :json)
end
