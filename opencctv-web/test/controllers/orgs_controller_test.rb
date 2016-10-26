require 'test_helper'

class OrgsControllerTest < ActionController::TestCase
  setup do
    @org = orgs(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:orgs)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create org" do
    assert_difference('Org.count') do
      post :create, org: { detail: @org.detail, org_user_id: @org.org_user_id, password: @org.password, title: @org.title }
    end

    assert_redirected_to org_path(assigns(:org))
  end

  test "should show org" do
    get :show, id: @org
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @org
    assert_response :success
  end

  test "should update org" do
    patch :update, id: @org, org: { detail: @org.detail, org_user_id: @org.org_user_id, password: @org.password, title: @org.title }
    assert_redirected_to org_path(assigns(:org))
  end

  test "should destroy org" do
    assert_difference('Org.count', -1) do
      delete :destroy, id: @org
    end

    assert_redirected_to orgs_path
  end
end
