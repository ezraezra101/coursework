require 'Namey'
generator = Namey::Generator.new

def city
	# Census municipalities in Massachussetts.
	cities = ['Acushnet Center', 'Adams', 'Amesbury', 'Amherst Center', 'Andover', 'Athol', 'Ayer', 'Baldwinville', 'Barre', 'Belchertown', 'Bellingham', 'Bliss Corner', 'Bondsville', 'Bourne', 'Boxford', 'Brewster', 'Bridgewater', 'Buzzards Bay', 'Chatham', 'Clinton', 'Cochituate', 'Cordaville', 'Dennis', 'Dennis Port', 'Dover', 'Duxbury', 'East Brookfield', 'East Dennis', 'East Douglas', 'East Falmouth', 'East Harwich', 'East Pepperell', 'East Sandwich', 'Essex', 'Falmouth', 'Fiskdale', 'Forestdale', 'Fort Devens', 'Fort Devens', 'Foxborough', 'Granby', 'Great Barrington', 'Green Harbor', 'Greenfield', 'Groton', 'Hanson', 'Harwich Center', 'Harwich Port', 'Hatfield', 'Hingham', 'Holland', 'Hopedale', 'Hopkinton', 'Housatonic', 'Hudson', 'Ipswich', 'Kingston', 'Lee', 'Lenox', 'Littleton Common', 'Lunenburg', 'Mansfield Center', 'Marion Center', 'Marshfield', 'Marshfield Hills', 'Mashpee Neck', 'Mattapoisett Center', 'Medfield', 'Middleborough Center', 'Milford', 'Millers Falls', 'Millis-Clicquot', 'Monomoscoy Island', 'Monson Center', 'Monument Beach', 'Nantucket', 'New Seabury', 'North Amherst', 'North Attleborough Center', 'North Brookfield', 'North Eastham', 'North Falmouth', 'North Lakeville', 'North Pembroke', 'North Plymouth', 'North Scituate', 'North Seekonk', 'North Westport', 'Northborough', 'Northfield', 'Northwest Harwich', 'Norton Center', 'Ocean Bluff-Brant Rock', 'Ocean Grove', 'Onset', 'Orange', 'Orleans', 'Oxford', 'Palmer', 'Pepperell', 'Pinehurst', 'Plymouth Center', 'Pocasset', 'Popponesset Island', 'Popponesset', 'Provincetown', 'Raynham Center', 'Rockport', 'Rowley', 'Rutland', 'Sagamore', 'Salisbury', 'Sandwich', 'Scituate', 'Seabrook', 'Seconsett Island', 'Sharon', 'Shelburne Falls', 'Shirley', 'Smith Mills', 'South Amherst', 'South Ashburnham', 'South Deerfield', 'South Dennis', 'South Duxbury', 'South Lancaster', 'South Yarmouth', 'Spencer', 'Sturbridge', 'Teaticket', 'Three Rivers', 'Topsfield', 'Townsend', 'Turners Falls', 'Upton-West Upton', 'Vineyard Haven', 'Walpole', 'Ware', 'Wareham Center', 'Warren', 'Webster', 'Wellesley', 'West Brookfield', 'West Chatham', 'West Concord', 'West Dennis', 'West Falmouth', 'West Wareham', 'West Yarmouth', 'Westborough', 'Weweantic', 'White Island Shores', 'Whitinsville', 'Wilbraham', 'Williamstown', 'Wilmington', 'Winchendon', 'Woods Hole', 'Yarmouth Port']
	cities[rand(cities.length)]
end

for i in 0..10
	name = generator.name.split(' ');
	age = rand(20)+5
	puts "INSERT INTO TravelingWith VALUES ('#{i+123}', '#{11}', '#{name[0]}', '#{name[1]}', #{age});"
end


