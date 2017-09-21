# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20170821083112) do

  create_table "analytic_configs", force: true do |t|
    t.string   "name"
    t.string   "fileName"
    t.text     "data",        limit: 16777215
    t.integer  "analytic_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_configs", ["analytic_id"], name: "index_analytic_configs_on_analytic_id", using: :btree

  create_table "analytic_input_streams", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "analytic_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_input_streams", ["analytic_id"], name: "index_analytic_input_streams_on_analytic_id", using: :btree

  create_table "analytic_instance_configs", force: true do |t|
    t.string   "name"
    t.string   "fileName"
    t.text     "data",                 limit: 16777215
    t.integer  "analytic_instance_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_instance_configs", ["analytic_instance_id"], name: "index_analytic_instance_configs_on_analytic_instance_id", using: :btree

  create_table "analytic_instance_results_apps", force: true do |t|
    t.integer  "analytic_instance_id"
    t.integer  "results_app_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_instance_results_apps", ["analytic_instance_id"], name: "index_analytic_instance_results_apps_on_analytic_instance_id", using: :btree
  add_index "analytic_instance_results_apps", ["results_app_id"], name: "index_analytic_instance_results_apps_on_results_app_id", using: :btree

  create_table "analytic_instance_streams", force: true do |t|
    t.integer  "analytic_instance_id"
    t.integer  "analytic_input_stream_id"
    t.integer  "stream_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.text     "config"
  end

  add_index "analytic_instance_streams", ["analytic_input_stream_id"], name: "index_analytic_instance_streams_on_analytic_input_stream_id", using: :btree
  add_index "analytic_instance_streams", ["analytic_instance_id"], name: "index_analytic_instance_streams_on_analytic_instance_id", using: :btree
  add_index "analytic_instance_streams", ["stream_id"], name: "index_analytic_instance_streams_on_stream_id", using: :btree

  create_table "analytic_instances", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "analytic_id"
    t.integer  "notification_id"
    t.integer  "user_id"
    t.integer  "group_user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "status",             default: 0
    t.integer  "analytic_server_id"
  end

  add_index "analytic_instances", ["analytic_id"], name: "index_analytic_instances_on_analytic_id", using: :btree
  add_index "analytic_instances", ["analytic_server_id"], name: "index_analytic_instances_on_analytic_server_id", using: :btree
  add_index "analytic_instances", ["group_user_id"], name: "index_analytic_instances_on_group_user_id", using: :btree
  add_index "analytic_instances", ["notification_id"], name: "index_analytic_instances_on_notification_id", using: :btree
  add_index "analytic_instances", ["user_id"], name: "index_analytic_instances_on_user_id", using: :btree

  create_table "analytic_servers", force: true do |t|
    t.string   "name"
    t.string   "ip"
    t.integer  "port"
    t.string   "tag"
    t.string   "status"
    t.integer  "pid"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_servers", ["ip", "port"], name: "index_analytic_servers_on_ip_and_port", unique: true, using: :btree
  add_index "analytic_servers", ["name"], name: "index_analytic_servers_on_name", unique: true, using: :btree

  create_table "analytics", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.string   "filename"
    t.boolean  "verified",    default: false, null: false
    t.datetime "created_at"
    t.datetime "updated_at"
    t.text     "log"
  end

  create_table "api_session_tokens", force: true do |t|
    t.string   "token"
    t.integer  "user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "api_session_tokens", ["user_id"], name: "index_api_session_tokens_on_user_id", using: :btree

  create_table "auth_tokens", force: true do |t|
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "cameras", force: true do |t|
    t.string   "name"
    t.string   "camera_id"
    t.text     "description"
    t.boolean  "verified",    default: false, null: false
    t.integer  "vms_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "ip"
    t.integer  "port"
    t.string   "username"
    t.string   "password"
    t.integer  "http_port"
    t.integer  "video_port"
  end

  add_index "cameras", ["vms_id"], name: "index_cameras_on_vms_id", using: :btree

  create_table "google_projects", force: true do |t|
    t.string   "application_name"
    t.string   "project_number"
    t.string   "api_key"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "google_tokens", force: true do |t|
    t.string   "reg_token"
    t.integer  "analytic_id"
    t.integer  "user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "google_tokens", ["user_id"], name: "index_google_tokens_on_user_id", using: :btree

  create_table "group_users", force: true do |t|
    t.string   "title"
    t.text     "detail"
    t.integer  "org_id"
    t.integer  "role_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "group_users", ["org_id"], name: "index_group_users_on_org_id", using: :btree
  add_index "group_users", ["role_id"], name: "index_group_users_on_role_id", using: :btree

  create_table "mobile_notifications", force: true do |t|
    t.integer  "analytic_id"
    t.integer  "google_project_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "mobile_notifications", ["analytic_id"], name: "index_mobile_notifications_on_analytic_id", using: :btree
  add_index "mobile_notifications", ["google_project_id"], name: "index_mobile_notifications_on_google_project_id", using: :btree

  create_table "notifications", force: true do |t|
    t.string   "title"
    t.text     "email"
    t.text     "to_user"
    t.integer  "user_id"
    t.integer  "group_user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "notifications", ["group_user_id"], name: "index_notifications_on_group_user_id", using: :btree
  add_index "notifications", ["user_id"], name: "index_notifications_on_user_id", using: :btree

  create_table "open_cctv_servers", force: true do |t|
    t.string   "name"
    t.string   "host"
    t.integer  "port"
    t.string   "status"
    t.integer  "pid"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "org_group_users", force: true do |t|
    t.integer  "org_user_id"
    t.integer  "group_user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "org_group_users", ["group_user_id"], name: "index_org_group_users_on_group_user_id", using: :btree
  add_index "org_group_users", ["org_user_id"], name: "index_org_group_users_on_org_user_id", using: :btree

  create_table "org_users", force: true do |t|
    t.integer  "user_id"
    t.integer  "org_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "org_users", ["org_id"], name: "index_org_users_on_org_id", using: :btree
  add_index "org_users", ["user_id"], name: "index_org_users_on_user_id", using: :btree

  create_table "orgs", force: true do |t|
    t.string   "title"
    t.text     "detail"
    t.string   "password"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "created_by"
  end

  create_table "requests", force: true do |t|
    t.integer  "status_id"
    t.integer  "org_id"
    t.integer  "user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "requests", ["org_id"], name: "index_requests_on_org_id", using: :btree
  add_index "requests", ["status_id"], name: "index_requests_on_status_id", using: :btree
  add_index "requests", ["user_id"], name: "index_requests_on_user_id", using: :btree

  create_table "results", force: true do |t|
    t.string   "result_text"
    t.string   "location"
    t.time     "timestamp"
    t.integer  "analytic_instance_id"
    t.integer  "analytic_id"
    t.integer  "vms_id"
    t.integer  "camera_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "results", ["analytic_id"], name: "index_results_on_analytic_id", using: :btree
  add_index "results", ["analytic_instance_id"], name: "index_results_on_analytic_instance_id", using: :btree
  add_index "results", ["camera_id"], name: "index_results_on_camera_id", using: :btree
  add_index "results", ["vms_id"], name: "index_results_on_vms_id", using: :btree

  create_table "results_app_connector_files", force: true do |t|
    t.string   "name"
    t.boolean  "required"
    t.text     "description"
    t.integer  "results_app_connector_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.boolean  "editable"
  end

  add_index "results_app_connector_files", ["results_app_connector_id"], name: "index_results_app_connector_files_on_results_app_connector_id", using: :btree

  create_table "results_app_connector_parameters", force: true do |t|
    t.string   "name"
    t.boolean  "required"
    t.boolean  "editable"
    t.text     "description"
    t.integer  "results_app_connector_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "results_app_connector_parameters", ["results_app_connector_id"], name: "index_r_app_connector_params_on_r_app_connector_id", using: :btree

  create_table "results_app_connectors", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.string   "filename"
    t.boolean  "verified"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.text     "log"
  end

  create_table "results_app_input_files", force: true do |t|
    t.integer  "results_app_id"
    t.integer  "results_app_connector_file_id"
    t.string   "filename"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "results_app_input_files", ["results_app_connector_file_id"], name: "index_results_app_input_files_on_results_app_connector_file_id", using: :btree
  add_index "results_app_input_files", ["results_app_id"], name: "index_results_app_input_files_on_results_app_id", using: :btree

  create_table "results_app_input_parameters", force: true do |t|
    t.integer  "results_app_id"
    t.integer  "results_app_connector_parameter_id"
    t.string   "value"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "results_app_input_parameters", ["results_app_connector_parameter_id"], name: "index_r_app_input_params_on_r_app_connector_param_id", using: :btree
  add_index "results_app_input_parameters", ["results_app_id"], name: "index_results_app_input_parameters_on_results_app_id", using: :btree

  create_table "results_apps", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "results_app_connector_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "user_id"
    t.integer  "group_user_id"
    t.boolean  "usable"
  end

  add_index "results_apps", ["group_user_id"], name: "index_results_apps_on_group_user_id", using: :btree
  add_index "results_apps", ["results_app_connector_id"], name: "index_results_apps_on_results_app_connector_id", using: :btree
  add_index "results_apps", ["user_id"], name: "index_results_apps_on_user_id", using: :btree

  create_table "roles", force: true do |t|
    t.string   "title"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "statuses", force: true do |t|
    t.string   "status"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "streams", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "width"
    t.integer  "height"
    t.boolean  "keep_aspect_ratio"
    t.boolean  "allow_upsizing"
    t.integer  "compression_rate"
    t.boolean  "verified",          default: false, null: false
    t.integer  "camera_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "codec"
    t.string   "protocol"
    t.string   "url"
    t.integer  "fps"
  end

  add_index "streams", ["camera_id"], name: "index_streams_on_camera_id", using: :btree

  create_table "users", force: true do |t|
    t.string   "email",                  default: "", null: false
    t.string   "encrypted_password",     default: "", null: false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          default: 0,  null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.integer  "vms_id"
    t.integer  "notification_id"
    t.integer  "analytic_instance_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "token"
    t.string   "phone_number"
    t.boolean  "isAdmin"
    t.string   "username"
    t.string   "lastname"
  end

  add_index "users", ["analytic_instance_id"], name: "index_users_on_analytic_instance_id", using: :btree
  add_index "users", ["email"], name: "index_users_on_email", unique: true, using: :btree
  add_index "users", ["notification_id"], name: "index_users_on_notification_id", using: :btree
  add_index "users", ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true, using: :btree
  add_index "users", ["vms_id"], name: "index_users_on_vms_id", using: :btree

  create_table "vms_connectors", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.string   "filename"
    t.boolean  "verified",    default: false, null: false
    t.text     "log"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "vmses", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.string   "server_ip"
    t.integer  "server_port"
    t.string   "username"
    t.string   "password"
    t.boolean  "verified",            default: false, null: false
    t.integer  "vms_connector_id"
    t.integer  "user_id"
    t.integer  "group_user_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "vms_type"
    t.integer  "open_cctv_server_id"
    t.string   "connection_type"
    t.boolean  "isFrameGrabber"
  end

  add_index "vmses", ["group_user_id"], name: "index_vmses_on_group_user_id", using: :btree
  add_index "vmses", ["open_cctv_server_id"], name: "index_vmses_on_open_cctv_server_id", using: :btree
  add_index "vmses", ["user_id"], name: "index_vmses_on_user_id", using: :btree
  add_index "vmses", ["vms_connector_id"], name: "index_vmses_on_vms_connector_id", using: :btree

end
