class User < ActiveRecord::Base
  has_many :google_tokens

  #has_many :orgs

  has_many :requests, dependent: :destroy

  has_many :org_users, dependent: :destroy
  has_many :orgs, through: :org_users

  has_many :vmses, dependent: :destroy
  has_many :analytic_instances, dependent: :destroy
  has_many :notifications, dependent: :destroy

  belongs_to :role

  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :trackable, :validatable



  def fullName
    "#{username} #{lastname}"
  end

end
