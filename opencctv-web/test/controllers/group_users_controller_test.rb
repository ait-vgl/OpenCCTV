require 'test_helper'

class GroupUsersControllerTest < ActionController::TestCase
  setup do
    @group_user = group_users(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:group_users)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create group_user" do
    assert_difference('GroupUser.count') do
      post :create, group_user: { detail: @group_user.detail, org_group_user_id: @group_user.org_group_user_id, title: @group_user.title }
    end

    assert_redirected_to group_user_path(assigns(:group_user))
  end

  test "should show group_user" do
    get :show, id: @group_user
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @group_user
    assert_response :success
  end

  test "should update group_user" do
    patch :update, id: @group_user, group_user: { detail: @group_user.detail, org_group_user_id: @group_user.org_group_user_id, title: @group_user.title }
    assert_redirected_to group_user_path(assigns(:group_user))
  end

  test "should destroy group_user" do
    assert_difference('GroupUser.count', -1) do
      delete :destroy, id: @group_user
    end

    assert_redirected_to group_users_path
  end
end
