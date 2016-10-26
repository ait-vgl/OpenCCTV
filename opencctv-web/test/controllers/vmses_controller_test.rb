require 'test_helper'

class VmsesControllerTest < ActionController::TestCase
  setup do
    @vms = vmses(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:vmses)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create vms" do
    assert_difference('Vms.count') do
      post :create, vms: { description: @vms.description, name: @vms.name, password: @vms.password, server_ip: @vms.server_ip, server_port: @vms.server_port, username: @vms.username, verified: @vms.verified, vms_connector_id: @vms.vms_connector_id }
    end

    assert_redirected_to vms_path(assigns(:vms))
  end

  test "should show vms" do
    get :show, id: @vms
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @vms
    assert_response :success
  end

  test "should update vms" do
    patch :update, id: @vms, vms: { description: @vms.description, name: @vms.name, password: @vms.password, server_ip: @vms.server_ip, server_port: @vms.server_port, username: @vms.username, verified: @vms.verified, vms_connector_id: @vms.vms_connector_id }
    assert_redirected_to vms_path(assigns(:vms))
  end

  test "should destroy vms" do
    assert_difference('Vms.count', -1) do
      delete :destroy, id: @vms
    end

    assert_redirected_to vmses_path
  end
end
