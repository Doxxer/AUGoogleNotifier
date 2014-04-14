import webapp2

import view


class BaseHandler(webapp2.RequestHandler):
    def render(self, template):
        jinja_template = view.JINJA_ENVIRONMENT.get_template(template)
        self.response.write(jinja_template.render())