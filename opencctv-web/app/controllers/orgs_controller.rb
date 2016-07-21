class OrgsController < ApplicationController
  before_action :set_org, only: [:show, :edit, :update, :destroy]
  before_action :authenticate_user!
  before_action :isOrganizationPageAdmin?, only: [:edit, :update, :destroy, :indexUser, :removeUser]
  respond_to :html

  def index
    @orgs = Org.joins(:users).where(:users => {:id => current_user.id})
    respond_with(@orgs)
  end

  def show
    @group_users = @org.group_users
    respond_with(@org)
  end

  def new
    @org = Org.new
    respond_with(@org)
  end

  def edit
  end

  def create
    @org = Org.new(:title => params[:org][:title], :detail => params[:org][:detail], :password => params[:org][:password],
                    :created_by => current_user.id)
    if @org.save
      @org_user = @org.org_users.create(:user_id => @org.created_by)

      @group_user = @org.group_users.create(:title => 'Organization admin', :detail => 'This group is used for managing this organization.',
                                            :role_id => ApplicationController::ROLE_ORGANIZATION_ADMIN)

      @group_user.org_group_users.create(:org_user_id => @org_user.id)

      setSessionOrganizationId(@org.id.to_s)

    else
      flash[:error] = 'Cannot create an organization.'
    end
    respond_with(@org)
  end

  def update
    @org.update(org_params)
    respond_with(@org)
  end

  def destroy

    @org.destroy

    if @org.id.to_s == session[:org_id]
      setSessionOrganizationId(nil)
    end

    respond_with(@org)
  end

  def loginAsOrg
    setSessionOrganizationId(params[:org_id].to_s)
    redirect_to :back
  end

  def loginAsUser
    setSessionOrganizationId(nil)
    redirect_to root_path
  end

  def indexUser
    @org_users = User.joins(:orgs).where(:orgs => {:id => session[:org_id]})
  end

  def removeUser
    @org_user = OrgUser.where(:org_id => session[:org_id], :user_id => params[:user_id]).first()

    if @org_user.nil? or @org_user.blank?
      flash[:error] = 'Cannot remove user.'
    else
      @org_user.destroy

      flash[:notice] = 'User was removed.'
    end

    redirect_to org_users_path
  end

  private
    def set_org
      @org = Org.find(params[:id])
    end

    def org_params
      params.require(:org).permit(:title, :detail, :password)
    end
end
