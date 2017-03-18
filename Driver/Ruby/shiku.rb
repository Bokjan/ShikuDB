require 'json'
require 'net/http'
Net::HTTP.version_1_2
class Shiku
	public
	def initialize(h = '127.0.0.1', p = 6207)
		@host, @port, @url = h, p, "http://#{h}:#{p}/"
		@db, @collection = '', ''
	end
	def test
		
	end
	def db
		@db.empty? ? 'nil' : @db
	end
	def collection
		@collection.empty? ? 'nil' : @collection
	end
	def method_missing(method, *arguments, &block)
	
	end
	def connect
		post([{'operation' => 'ConnectionTest'}].to_json)
	end
	private
	def post(body)
		req = NET::HTTP::Post.(@url, {'Content-Type' => 'application/json'})
		req.body = data
		res = NET::HTTP.start(@host, @port) {|http|
			return http.post('/', body).body
		}
	end
	end
end