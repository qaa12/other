#qaa12
class GeneticAlg
	attr_reader :n, :popSize, :pC, :pM

	def initialize(n, popSize, pC, pM)
		@n = n
		@popSize = popSize
		@pC = pC
		@pM = pM
	end

	class Board 
		attr_accessor :hrom, :sum

		def initialize(n, hrom=nil)
			@sum = 0
			@hrom = hrom
			@hrom ||= Array.new(n)
			self.shuffle
		end

		def shuffle
			@hrom.map!{|x| x = rand(@hrom.size)}
		end

		def hrom_print
			@hrom.each{|x| print x+" "}
			puts
		end

		def mutation
			i = rand @hrom.size
			@hrom[i] = (@hrom[i] + rand(@hrom.size)) % @hrom.size
		end

		def to_s
			ans = ""
			for i in 0...@hrom.size
				for j in 0...@hrom.size
					ans += j == @hrom[i] ? 'x ' : '0 '
				end
                ans+="\n"
			end
            ans
		end

		def validate
			@sum = 0
			for i in 0...@hrom.size
				for j in i+1...@hrom.size
					@sum+=1 if (@hrom[i] == @hrom[j])
					@sum+=1 if (@hrom[j] == @hrom[i] + (j-i))
					@sum+=1 if (@hrom[j] == @hrom[i] - (j-i))
				end
			end
			@sum == 0
		end

	end #end Board

	class Population

		def initialize(settings)
			@settings = settings
			@maxSumFunc = 0
			@oldPop = Array.new(settings.popSize)
			@oldPop.map!{|x| x =  Board.new(settings.n)}
			@newPop = Array.new(settings.popSize, 0)
			settings.n.times{|x| @maxSumFunc += x}
			@sumFunc = 0
			@generation = 0
		end
		
		def validate
			@sumFunc = 0
			for i in 0...@settings.popSize
				if @oldPop[i].validate
					puts @oldPop[i]
					puts "generation: #{@generation}"
					return true
				end	
				@sumFunc += @oldPop[i].sum
			end
			   # puts "generation: #{@generation}"
                return false
		end

		def crossOver(parent1, parent2)
			newHrom = []
			if rand <= @settings.pC
				l = rand(@settings.n - 2) + 1
				for i in 0...@settings.n
					newHrom[i] = (i >= l ? parent1.hrom[i] : parent2.hrom[i]) 
				end
				return Board.new(@settings.n, newHrom)
			else
				return parent1
			end
		end

		def select
			random = rand
			selecter = 0
			for i in 0...@settings.popSize
				selecter += (@maxSumFunc - @oldPop[i].sum)/@sumFunc.to_f
				break if (random <= selecter)
			end
			return i
		end

		def doThis(i)
			id1 = select
			id2 = select
		    while( id1 == (id2 = select))
            end
			parent1 = @oldPop[id1]
			parent2 = @oldPop[id2]
			child1 = crossOver(parent1, parent2)
			child2 = crossOver(parent2, parent1)
			if rand >= @settings.pM
				child1.mutation
				child2.mutation
			end
			@newPop[i*2-1] = child1
			@newPop[i*2] = child2
		end

		def calc
			until validate
				for i in 0...@settings.popSize/2
					doThis(i)
				end
				@oldPop = @newPop.dup
				@generation+=1
			end
		end

	end #end Population

	def run
		pop = Population.new(self)
		pop.calc
	end

end

test = GeneticAlg.new(6, 50, 0.5, 0.5)
test2 = GeneticAlg.new(8, 50, 0.5, 0.5)
test.run
#test2.run
