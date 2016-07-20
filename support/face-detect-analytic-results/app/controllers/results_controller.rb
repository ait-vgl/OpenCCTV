class ResultsController < ApplicationController
  before_action :set_result, only: [:show, :destroy]
  # before_action :remove_duplicates, only: [:index]
  # before_action :process_new_results, only: [:index]

  after_action :run_vlc, only: [:show]

  # GET /results
  # GET /results.json
  def index
    remove_duplicates
    process_new_results
    @results = Result.all
    @results.each do |result|
      if(!result.time.nil?)
        images = Dir.glob("#{Rails.root}/app/assets/images/#{result.timestamp}/*.jpg")
        if(images.count == 0)
          result.send_save_images_request
        end
      end
    end
  end

  # GET /results/1
  # GET /results/1.json
  def show
  end

  # GET /results/new
  def new
    redirect_to results_path
  end

  # GET /results/1/edit
  def edit
    redirect_to results_path
  end

  # POST /results
  # POST /results.json
  def create
  end

  # PATCH/PUT /results/1
  # PATCH/PUT /results/1.json
  def update
  end

  # DELETE /results/1
  # DELETE /results/1.json
  def destroy
    @result.destroy
    respond_to do |format|
      format.html { redirect_to results_url, notice: 'Result was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_result
      @result = Result.find(params[:id])
    end

    def remove_duplicates
      all_duplicates = Result.all.group_by{|result| [result.analytic_instance_id, result.timestamp]}
      all_duplicates.values.each do |result|
        # the first one we want to keep right?
        first_one = result.shift # or pop for last one
        # if there are any more left, they are duplicates
        # so delete all of them
        result.each{|double| double.destroy} # duplicates can now be destroyed
      end
    end

    def process_new_results
      Result.group(:timestamp).where('time is null').each do |result|
        time = DateTime.iso8601(result.timestamp)
        time -= 2.seconds
        count = Nokogiri::XML(result.result_text).at('count').content
        result.update(time: time, count: count)
        path_to_save_images = File.join(Rails.root, 'app', 'assets', 'images', result.timestamp)
        system("mkdir -p #{path_to_save_images}")
        result.send_save_images_request
      end
    end

    def run_vlc
      system("vlc -vvv #{@result.get_rtsp_uri} &")
    end
end
