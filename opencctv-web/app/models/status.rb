class Status < ActiveRecord::Base
  has_many :requests
end
