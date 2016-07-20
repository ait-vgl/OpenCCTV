require 'test_helper'

class OpenCctvServersControllerTest < ActionController::TestCase
  setup do
    @open_cctv_server = open_cctv_servers(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:open_cctv_servers)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create open_cctv_server" do
    assert_difference('OpenCctvServer.count') do
      post :create, open_cctv_server: { host: @open_cctv_server.host, name: @open_cctv_server.name, pid: @open_cctv_server.pid, port: @open_cctv_server.port, status: @open_cctv_server.status }
    end

    assert_redirected_to open_cctv_server_path(assigns(:open_cctv_server))
  end

  test "should show open_cctv_server" do
    get :show, id: @open_cctv_server
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @open_cctv_server
    assert_response :success
  end

  test "should update open_cctv_server" do
    patch :update, id: @open_cctv_server, open_cctv_server: { host: @open_cctv_server.host, name: @open_cctv_server.name, pid: @open_cctv_server.pid, port: @open_cctv_server.port, status: @open_cctv_server.status }
    assert_redirected_to open_cctv_server_path(assigns(:open_cctv_server))
  end

  test "should destroy open_cctv_server" do
    assert_difference('OpenCctvServer.count', -1) do
      delete :destroy, id: @open_cctv_server
    end

    assert_redirected_to open_cctv_servers_path
  end
end
