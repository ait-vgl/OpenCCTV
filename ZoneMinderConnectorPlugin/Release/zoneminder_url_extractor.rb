require 'unirest'
require 'nokogiri'

def create_zoneminder_url(ip, port)
	if(port == 80)
		return "http://#{ip}/index.php"
	end
	return "http://#{ip}:#{port}/index.php"
end

# zoneminder login returns cookie (as an Array) after successful auth or nil if login failed
def zoneminder_login(url, username, password)
	cookie = nil # return val
    response = Unirest.post(url, parameters:{ :action => "login", :view => "postlogin", :username => username, :password => password })
    error_msg = nil
    if response.code == 200
      	if response.body.include? "username" and response.body.include? "password" #again login form
      	elsif(response.body.include? 'Logging In') #got auth correct
        	cookie = response.headers[:set_cookie]
      	else #unknown response
        	error_msg = 'Failed to connect to the ZoneMinder server. Unknown response from ZoneMinder server.'
      	end
    else
      	error_msg = 'Failed to connect to the ZoneMinder server.'
    end
    if(!error_msg.nil?)
      	raise error_msg
    end
    return cookie
end
  
# zoneminder validate monitor. returns true if valid
def zoneminder_validate_monitor(url, cookie, camera_id)
	response = Unirest.get(url, headers:{"Cookie" => cookie.join(" ")})
	if response.code == 200
		page = Nokogiri::HTML(response.body)
      	page.css('tbody//tr//td[class="colName"]').each do |monitorInfo|
        	monitorName = monitorInfo.inner_text
        	monitorWatchPath = monitorInfo.children[0].attribute("href").inner_text
        	if(monitorWatchPath == camera_id)
        		return true
        	end
      end
    else
    	raise 'Failed to connect to the ZoneMinder server to validate the Monitor.'
	end
end

# zoneminder get stream url. returns stream url if successful
def zoneminder_get_stream_url(url, cookie, camera_id)
	monitorWatchUrl = "#{url}?view=watch&mid=#{camera_id}"
	response = Unirest.post(monitorWatchUrl, headers:{"Cookie" => cookie.join(" ")})
	if(response.code == 200)
		page = Nokogiri::HTML(response.body)
      	streamInfo = page.css('div#imageFeed//img#liveStream')[0]
      	if(!streamInfo.nil?)
        	streamUrl = streamInfo.attribute("src").inner_text
        	return streamUrl
        else
        	raise 'Failed to retrieve stream URL.'
      	end
    else
    	raise 'Failed to connect to ZoneMinder server to retrieve stream URL.'
	end
end

# MAIN
err = true
output = nil
if(ARGV.length == 5)
	ip = ARGV[0].to_s
	port = ARGV[1].to_i
	username = ARGV[2].to_s
	password = ARGV[3].to_s
	camera_id = ARGV[4].to_s
	url = create_zoneminder_url(ip, port)
	begin
		cookie = zoneminder_login(url, username, password)
		if(!cookie.nil? && zoneminder_validate_monitor(url, cookie, camera_id))
			output = zoneminder_get_stream_url(url, cookie, camera_id)
			err = false
		else
			output = "Error: Failed to connect to ZoneMinder server."
		end
	rescue Exception => e
		output = "Error: #{e.message}"
	end
else
	output = 'Error: Invalid number of arguments.'
end
# generate output xml
puts output

