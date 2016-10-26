require 'test_helper'

class AnalyticServersControllerTest < ActionController::TestCase
  setup do
    @analytic_server = analytic_servers(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_servers)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_server" do
    assert_difference('AnalyticServer.count') do
      post :create, analytic_server: { ip: @analytic_server.ip, name: @analytic_server.name, pid: @analytic_server.pid, port: @analytic_server.port, status: @analytic_server.status }
    end

    assert_redirected_to analytic_server_path(assigns(:analytic_server))
  end

  test "should show analytic_server" do
    get :show, id: @analytic_server
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_server
    assert_response :success
  end

  test "should update analytic_server" do
    patch :update, id: @analytic_server, analytic_server: { ip: @analytic_server.ip, name: @analytic_server.name, pid: @analytic_server.pid, port: @analytic_server.port, status: @analytic_server.status }
    assert_redirected_to analytic_server_path(assigns(:analytic_server))
  end

  test "should destroy analytic_server" do
    assert_difference('AnalyticServer.count', -1) do
      delete :destroy, id: @analytic_server
    end

    assert_redirected_to analytic_servers_path
  end
end
