class GoogleProjectsController < ApplicationController
  def index
    @google_projects = GoogleProject.all
  end

  def new
    @google_project = GoogleProject.new
  end

  def create
    @google_project = GoogleProject.new(
      :application_name => params[:google_project][:application_name],
      :project_number => params[:google_project][:project_number],
      :api_key => params[:google_project][:api_key])
    @google_project.save
    if (!@google_project.errors.any?)
      flash[:notice] = "Google project is registered."
      redirect_to google_projects_path
    end
  end

  def show
  end

  def edit
  end

  def destroy
    google_project = GoogleProject.find_by_id(params[:id])
    google_project.destroy
    flash[:notice] = "Google project is unregistered."
    redirect_to google_projects_path
  end
end
