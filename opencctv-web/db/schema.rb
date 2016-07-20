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

ActiveRecord::Schema.define(version: 20150602024820) do

  create_table "analytic_input_streams", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "analytic_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_input_streams", ["analytic_id"], name: "index_analytic_input_streams_on_analytic_id", using: :btree

  create_table "analytic_instance_streams", force: true do |t|
    t.integer  "analytic_instance_id"
    t.integer  "analytic_input_stream_id"
    t.integer  "stream_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_instance_streams", ["analytic_input_stream_id"], name: "index_analytic_instance_streams_on_analytic_input_stream_id", using: :btree
  add_index "analytic_instance_streams", ["analytic_instance_id"], name: "index_analytic_instance_streams_on_analytic_instance_id", using: :btree
  add_index "analytic_instance_streams", ["stream_id"], name: "index_analytic_instance_streams_on_stream_id", using: :btree

  create_table "analytic_instances", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.integer  "analytic_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "analytic_instances", ["analytic_id"], name: "index_analytic_instances_on_analytic_id", using: :btree

  create_table "analytics", force: true do |t|
    t.string   "name"
    t.text     "description"
    t.string   "filename"
    t.boolean  "verified",    default: false, null: false
    t.datetime "created_at"
    t.datetime "updated_at"
    t.text     "log"
  end

  create_table "cameras", force: true do |t|
    t.string   "name"
    t.string   "camera_id"
    t.text     "description"
    t.boolean  "verified",    default: false, null: false
    t.integer  "vms_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "cameras", ["vms_id"], name: "index_cameras_on_vms_id", using: :btree

  create_table "open_cctv_servers", force: true do |t|
    t.string   "name"
    t.string   "host"
    t.integer  "port"
    t.string   "status"
    t.integer  "pid"
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
  end

  add_index "streams", ["camera_id"], name: "index_streams_on_camera_id", using: :btree

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
    t.boolean  "verified",         default: false, null: false
    t.integer  "vms_connector_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "vms_type"
  end

  add_index "vmses", ["vms_connector_id"], name: "index_vmses_on_vms_connector_id", using: :btree

end
