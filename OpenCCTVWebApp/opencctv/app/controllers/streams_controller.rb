class StreamsController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_stream, only: [:show, :edit, :update, :destroy]
  before_action :set_camera, only: [:index, :new, :create, :show, :edit, :destroy]
  before_action :set_vms, only: [:new, :show, :edit, :destroy]
  respond_to :html

  def index
    redirect_to vms_camera_path(@camera.vms, @camera)
  end

  def show
    respond_with(@stream)
  end

  def new
    @stream = Stream.new
    @stream.camera = @camera
    respond_with(@stream)
  end

  def create
    @stream = @camera.streams.create(:name => params[:stream][:name], :width => params[:stream][:width], :height => params[:stream][:height], :keep_aspect_ratio => params[:stream][:keep_aspect_ratio], :allow_upsizing => params[:stream][:allow_upsizing], :compression_rate => params[:stream][:compression_rate], :description => params[:stream][:description])
    if (!@stream.errors.any?)
      if (@stream.camera.vms.vms_type == "Milestone")
        if(@stream.milestone_validate_stream)
          @stream.set_verification(true)
          is_stream_valid, width, height = @stream.milestone_grab_stream_frame
        else
          @stream.set_verification(false)
        end
      else
      end
      redirect_to vms_camera_stream_path(@stream.camera.vms, @stream.camera, @stream)
    else
      redirect_to edit_vms_camera_stream_path(@stream)
    end
  end

  def edit
  end

  def update
    @stream.update(:name => params[:stream][:name], :width => params[:stream][:width], :height => params[:stream][:height], :keep_aspect_ratio => params[:stream][:keep_aspect_ratio], :allow_upsizing => params[:stream][:allow_upsizing], :compression_rate => params[:stream][:compression_rate], :description => params[:stream][:description])
    if (!@stream.errors.any?)
      if (@stream.camera.vms.vms_type == "Milestone")
        if(@stream.milestone_validate_stream)
          @stream.set_verification(true)
          is_stream_valid, width, height = @stream.milestone_grab_stream_frame
        else
          @stream.set_verification(false)
        end
      else
      end
      redirect_to vms_camera_stream_path(@stream.camera.vms, @stream.camera, @stream)
    else
      redirect_to edit_vms_camera_stream_path(@stream)
    end
  end

  def destroy
    @stream.destroy
    redirect_to vms_camera_path(@vms, @camera)
  end

  private

  def set_stream
    @stream = Stream.find(params[:id])
  end

  def set_camera
    @camera = Camera.find(params[:camera_id])
  end

  def set_vms
    @vms = Vms.find(params[:vms_id])
  end
end
