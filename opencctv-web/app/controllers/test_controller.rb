class TestController < ApplicationController
  def createTest
   number = 50
   #  params.permit(:number)
   #  puts(params[test][:number])
    #puts(params[:test][:number])

    vms = Vms.first()

    for i in 1..number
      @camera = Camera.new
      @camera.name = "Camera #{i}"
      @camera.camera_id="950CDEBC-5784-499E-8243-BF114D9C0ED3"
      @camera.description="NULL"
      @camera.verified="1"
      @camera.vms_id=vms.id

      @camera.save()


    # puts(@camera.id)

      @stream = Stream.new
      @stream.name="Default - Camera #{i}"
      @stream.description="Original Stream from Camera"
      @stream.width="642"
      @stream.height="336"
      @stream.keep_aspect_ratio="0"
      @stream.allow_upsizing="0"
      @stream.compression_rate="90"
      @stream.verified="1"
      @stream.camera_id=@camera.id

      @stream.save()


      # analytic instance
      @analytic_instance = AnalyticInstance.new
      @analytic_instance.name="analytic_instance #{i}"
      @analytic_instance.analytic_id="21"
      @analytic_instance.notification_id="1"
      @analytic_instance.user_id="2"
      @analytic_instance.group_user_id="NULL"

      @analytic_instance.save()

      @analytic_instance_stream = AnalyticInstanceStream.new
      @analytic_instance_stream.analytic_instance_id=@analytic_instance.id
      @analytic_instance_stream.analytic_input_stream_id="15"
      @analytic_instance_stream.stream_id=@stream.id

      @analytic_instance_stream.save()

    end

    flash[:notice] = 'Successfully created.'
    redirect_to root_path


  end

  def clearTest
    Camera.delete_all()
    Stream.delete_all()
    AnalyticInstance.delete_all()
    AnalyticInstanceStream.delete_all()

    flash[:notice] = 'Successfully destroyed.'
    redirect_to root_path
  end


end
