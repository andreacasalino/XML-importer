/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _XML_TAG_H_
#define _XML_TAG_H_

#include <map>
#include <memory>
#include <vector>
#include <ostream>
#include <string>
#include <functional>

namespace xmlPrs {
	class Parser;

   /** @brief The object storing the data of an xml like structure
	 */
	class Tag {
	public:
	   /** @brief A new root is created, i.e. tag with no parent
		 * @param The name to assume for the tag to create 
	 	 */
		explicit Tag(const std::string& name); 
	   /** @brief The content of the passed tag is copied in
		 * a new root tag.
	 	 */
		explicit Tag(const Tag& o);
		Tag& operator=(const Tag& o); 

		friend std::ostream& operator<<(std::ostream&, const Tag&);

		typedef std::unique_ptr<Tag> TagPtr;
		typedef std::shared_ptr<std::string> TagName;

	   /** @return false in case the tag is a root
	 	 */
		bool hasFather() const;
	   /** @return the name of tag containing this one.
		 * @throw if the tag is a root and has not father
	 	*/
		const Tag& getFatherConst() const;
		Tag& getFather();

		/** @return the name of this tag
	 	 */
		inline const std::string& getName() const { return *this->name; };

		template<typename iterator_T>
		class Iterator_T;
		typedef Iterator_T<std::multimap<TagName, TagPtr>::iterator> Iterator;
		typedef Iterator_T<std::multimap<TagName, TagPtr>::const_iterator> ConstIterator;

		/** @return an iterator capable of scrolling all the tags nested in this one
	 	 */
		Iterator getNestedAll();
		ConstIterator getNestedAllConst() const;

	    /** @return an iterator capable of scrolling all the tags nested in this one,
		 * with a name equal to name.
		 * @param the name of the tags to iterate
	 	 */
		Iterator getNested(const std::string& name);
		ConstIterator getNestedConst(const std::string& name) const;

		/** @brief Get a tag in a specified position, starting from this tag.
		 * The term position refers to a chain of tag names. Essentially, the path is
		 * the series of tag to traverse to get a certain nested tag.
		 * Consider this example:
		 * <A><B><C><D></D></C></B></A>.
		 * The path of tag D from A is: {B,C,D}. At the same time, the path of tag D from
		 * B is: {C,D}. Therefore, If tou wanna get D using Tag_readable::Get_Nested(const std::list<std::string>& position) on Tag wrapping A,
		 * you should pass as input {B,C,D}. Instead, when you want to get D from a Tag_readable wrapping B, you should pass as input {C,D}.
		 * @param[in] the sequence of tag to traverse to get the one of interest from the one wrapped
		 * @return the desired tag
		 * @throw if the tag in the passed location does not exist
		 */
		const Tag& getNestedConst(const std::vector<std::string>& position) const; 
		Tag&       getNested(const std::vector<std::string>& position); 

	    /** @return a reference to the attributes contained in this tag.
	 	 */
		inline const std::multimap<std::string, std::string>& getAttributesConst() const { return this->fields; };
		inline std::multimap<std::string, std::string>& getAttributes() { return this->fields; };

		/** @brief set the name of this tag.
	 	 */
		void setName(const std::string& new_name);

		/** @brief change the names of all the attributes matching the passed name, 
		 * matching the passed name.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void setAttributeName(const std::string& name_attribute, const std::string& new_name_attribute);

		/** @brief change the names of all the attributes matching the passed name 
		 * and the value.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the value of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void setAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute);

		/** @brief remove this tag from the parent.
		 * @throw if this tag is a root.
	 	 */
		void remove();

		/** @brief removes all the tags nested by this object.
	 	 */
		void removeNestedAll();

		/** @brief adds a nested tag with the passed name and returns
		 * a reference to the newly created tag.
		 * @param[in] the name of the tag to add
		 * @return the newly created tag.
	 	 */
		Tag& addNested(const std::string& tag_name); 
		/** @brief adds a nested tag copying the passed one and returns
		 * a reference to the newly created tag.
		 * @param[in] the name of the tag to add
		 * @return the newly created tag.
	 	 */
		Tag& addNested(const Tag& structure);

		class Emplacer {
			friend class Parser;
			static void emplaceNested(Tag& recipient, Tag::TagPtr toNest);
		};
	private:
		void reprint(std::ostream& stream_to_use, const std::string& space_to_use) const;

	// data
		Tag*									  											father;
		TagName									  											name;
		std::multimap<std::string, std::string>   											fields;
		std::multimap<TagName, TagPtr, std::function<bool(const TagName&, const TagName&)>> nested;
	};

	std::ostream& operator<<(std::ostream& s, const Tag& t);
	
	/** @brief This object can be used to iterate a subset
	 * (or all) the tags nested in a parent.
	 * You can do this by creating an iterator equal to the one returned by begin(),
	 * and increment it till the one returned by end()
	 */
	template<typename iterator_T>
	class Tag::Iterator_T {
	public:
		Iterator_T(const iterator_T& begin, const iterator_T& end)
			: _begin(begin)
			, _end(end) {
		};

		inline std::size_t size() const { return std::distance(this->_begin , this->_end); };
        inline const iterator_T& begin() const { return this->_begin; };
        inline const iterator_T& end() const { return this->_end; };

	private:
		const iterator_T    _begin;
		const iterator_T    _end;
	};
}

#endif
