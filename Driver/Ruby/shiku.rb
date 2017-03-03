require 'json'
require 'net/http'
Net::HTTP.version_1_2
class Shiku
	public
	def initialize(h = '127.0.0.1', p = 6207)
		@host, @port = h, p
		@db, @collection = '', ''
	end
	def test
		
	end
	def db
		@db.empty? ? 'nil' : @db
	end
	def collection
		@collection.empty? ? 'nil' : @collection
	private
	def post(body)
		Net::HTTP.start(@host, @port) {|http|
			return http.post('/', body).body
		}
	end
end
