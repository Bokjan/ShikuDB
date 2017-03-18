require 'json'
require 'net/http'
class Shiku
	public
	def initialize(h = '127.0.0.1', p = 6207)
		@host, @port = h, p
		@db, @collection = '', ''
	end
	def test
		
	end
	def method_missing(method, *arguments, &block)
	
	end
	def connect
		ret = http_post({'operation' => 'ConnectionTest'})
		return true if ret['ok']
		ret['message']
	end
	def show_databases
		ret = http_post({'operation' => 'ShowDatabases'})
		return ret['databases'] if ret['ok']
		ret['message']
	end
	def use_database(db)
		ret = http_post({
			'operation' => 'UseDatabase',
			'database' => db
		})
		if ret['ok']
			@database = db
			return true
		end
		ret['message']
	end
	private
	def http_post(body)
		begin
			Net::HTTP.version_1_2
			Net::HTTP.start(@host, @port) {|http|
				# puts body.to_json
				response = http.post('/', body.to_json)
				return JSON.parse(response.body)
			}
		rescue Exception
			return {'ok' => 'false', 'message' => 'HTTP Error'}
		end
	end
end