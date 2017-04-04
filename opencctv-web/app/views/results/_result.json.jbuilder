json.extract! result, :id, :result_text, :location, :timestamp, :analytic_instance_id, :analytic_id, :vms_id, :camera_id, :created_at, :updated_at
json.url result_url(result, format: :json)