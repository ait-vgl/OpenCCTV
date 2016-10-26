class AnalyticInstanceStream < ActiveRecord::Base
  belongs_to :analytic_instance
  belongs_to :analytic_input_stream
  belongs_to :stream

  validates :analytic_instance_id, presence: true
  validates :stream_id, presence: true
  validates :analytic_input_stream_id, presence: true

  validates :analytic_input_stream_id, uniqueness: { scope: :analytic_instance_id, message: "for this input type is already configured for this analytic instance" }
end
